<?php

/**
 * @file
 * Batch file.
 */

function migrate_cleanup_rebuild_batch_process_entity($entity_id, &$context) {
  $entity = entity_load('node', array($entity_id), array(), TRUE);
  $node_wrapper = entity_metadata_wrapper('node', $entity);

  $content = $node_wrapper->body->value->value();

  // Set body value.
  $body = [
    'value' => filter_xss_admin($content),
    'format' => 'rich_text',
  ];

  $node_wrapper->body->set($body);
  $node_wrapper->save();

  // Update batch progress information.
  if (!isset($context['results']['processed'])) {
    $context['results']['processed'] = 0;
  }
  $context['results']['processed']++;
  $context['message'] = t('We are processing the entity @title with id: @id', array(
    '@title' => $node_wrapper->label(),
    '@id' => $entity_id,
  ));
}

/**
 * Batch finished.
 *
 * @param $success
 * @param $results
 * @param $operations
 */
function migrate_cleanup_rebuild_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = t('The contents has been cleaned and fixed.');
  }
  else {
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array(
      '%error_operation' => $error_operation[0],
      '@arguments' => print_r($error_operation[1], TRUE),
    ));
  }

  // Clear cache.
  cache_clear_all();

  drupal_set_message($message);
}